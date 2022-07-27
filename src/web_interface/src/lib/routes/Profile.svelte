<script lang="ts">
    import { onMount } from 'svelte'
    //import { getUser, updateUser, deleteUser } from '../logic'
    import { navigate } from "svelte-navigator"
    import profileImage from "../../assets/profile.png";
    import { fade } from 'svelte/transition'
    let email : string
    let name : string
    let surname : string
    let password : string
    let newPassword : string
    let prontogramUsername : string
    let disabled : boolean = true
    /*onMount(async () => {
        let response = await getUser()
        if(response != null){
            email = response.email
            name = response.name
            surname = response.surname
            //password = response.password
            prontogramUsername = response.prontogramUsername
            disabled = false
        }
	})
    function validate() : boolean {
        if(password.trim().length == 0)
            return false
        return true
    }
    async function handleSubmit(event): Promise<void> {
        if(!validate())
            return;
        const {submitter: submitButton} = event;
        if(submitButton.id == "save") {
            await updateUser(name, surname, email, password, newPassword == "" ?
                                             null : newPassword, prontogramUsername)
        } else if(submitButton.id == "delete"){
            await deleteUser(email, password)
            navigate("/")
        }
    }*/
    function handleSubmit(event){}
</script>

<div class="form" on:submit|preventDefault={handleSubmit}>
    <img class="mb-3 mt-4" src={profileImage} alt="profile" height="120" in:fade = {{duration: 200}} />
    <h1 class="h3 fw-normal">Your profile</h1>
    <form in:fade = {{duration: 200}}>
        <div class="mb-3">
            <label for="_" class="form-label">Nome</label>
            <input bind:value={name} type="Text" class="form-control" >
        </div>
        <div class="mb-3">
            <label for="_" class="form-label">Cognome</label>
            <input bind:value={surname} type="Text" class="form-control" >
        </div>
        <div class="mb-3">
            <label for="_" class="form-label">Email</label>
            <input bind:value={email} type="email" class="form-control" readonly>
        </div>
        <div class="mb-3">
            <label for="_" class="form-label">Password</label>
            <input bind:value={password} type="password" class="form-control" placeholder="" required disabled = {disabled}>
        </div>
        <div class="mb-3">
            <label for="_" class="form-label">Nuova password</label>
            <input bind:value={newPassword} type="password" class="form-control" placeholder="" disabled = {disabled}>
        </div>
        <div class="mb-3">
            <label for="_" class="form-label">Prontogram username</label>
            <input bind:value={prontogramUsername} type="text" class="form-control" disabled = {disabled}>
        </div>
        <button id="save" class="w-100 mb-3 mt-3 btn btn-primary" type="submit" disabled = {disabled}>Salva</button>
        <hr class="mb-3 dropdown-divider"/>
        <button id="delete" class="w-100 btn btn-danger" type="submit" disabled = {disabled} >Elimina account</button>
    </form>
</div>

<style>
    .form {
        width: 100%;
        display: flex;
        align-items: center;
        flex-direction: column;
        justify-content: flex-start;
    }
    form {
        width: 100%;
        max-width: 350px;
        padding: 30px;
    }
</style>